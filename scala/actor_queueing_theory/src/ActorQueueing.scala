import scala.concurrent.duration._
import akka.actor._
import akka.event.Logging

object ActorQueueing {

  def main(argv: Array[String]) {

    val system = ActorSystem("ActorQueueing")

    val inputRate = 200.millis
    val jobCost   = 100.millis
    val overhead  = 2.seconds

    val source = system actorOf Props(classOf[Source], inputRate)
    val worker = system actorOf Props(classOf[Worker], jobCost)
    val manager = system actorOf Props(classOf[Manager], source, worker, overhead)

    readLine("")

    system.shutdown()
  }
}

object Messages {

  case object Batch

  case class Job(nJobs: Int)
}

class Source(val frequency: FiniteDuration) extends Actor {

  import Messages._
  import context._

  case object Enqueue

  var enqueued: Int = 0

  val log = Logging(system, this)

  override def preStart() {
    system.scheduler schedule (0.seconds, frequency, self, Enqueue)
  }

  override def receive = {

    case Enqueue =>
      enqueued += 1
      log info s"$enqueued jobs"

    case Batch =>
      log info s"sending $enqueued jobs to $sender"
      sender ! Job(enqueued)
      enqueued = 0
  }
}

class Worker(val time: FiniteDuration) extends Actor {

  import Messages._
  import context._

  override def receive = {
    case Job(nJobs) =>
      system.scheduler scheduleOnce (time * nJobs, sender, Batch)
  }
}

class Manager(val source: ActorRef,
              val worker: ActorRef,
              val overhead: FiniteDuration) extends Actor {

  import Messages._
  import context._

  override def preStart() {
    source ! Batch
  }

  override def receive = {

    case Batch =>
      source ! Batch

    case j: Job =>
      system.scheduler scheduleOnce (overhead, worker, j)
  }
}

// questions:
//  - how can the system find its own stable point ?
//  - study the rate of stabilisation
