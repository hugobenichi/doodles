import java.util.*;


/*
 *  Time to solve:
 *
 *  About 45 minutes to sketch a solution
 *
 *  About 1 hour to try find the good datastructure I wanted in the Javadoc,
 *  because I have not used Java in a some time and was not sure about all the
 *  details of the standard classes.
 *
 *  About 1 hour to code it.
 *
 *  About 1.5 hour for checking syntax, explanations and comments
 *
 */
public class SeatingManager {


    public class CustomerGroup {
        public final int size;
        public CustomerGroup(int size) { this.size = size; }
    }


    public class Table {
        public final int size;
        public Table(int size) { this.size = size; }
    }


    /*
     *  This class wraps a Table instance and add a mutable field for keeping
     *  track of the number of free seats at the table.
     */
    public class OccupiedTable {
        public final Table table;
        public int free_seats;
        public OccupiedTable(Table table){
            this.table = table;
            free_seats = table.size;
        }
        /*  We use this comparison method for PriorityQueue */
        public int compareTo(OccupiedTable other){
            return this.table.size - other.table.size;
        }
    }


    /*  We keep all groups of customers as key in a map. The value indicates the
     *  table they are seated at. If they are still waiting for a table, the
     *  value is null. It allows for fast query of the table of any group.
     *  Insertion is O(1) and search or delete is O(1 + n/k) where n is the
     *  number of groups and k the number of buckets in the the hashmap.
     */
    private Map<CustomerGroup,OccupiedTable> all_groups;

    /*
     *  Groups not yet seated are put in a FIFO. It gives O(1) insertion/delete
     *  at the beginning and the end. It takes memory proportional to the number
     *  of group waiting.
     */
    private List<CustomerGroup> waiting_groups;

    /*
     *  To find seats quickly for new groups, we index tables by their current
     *  number of free seats. Because this number is bounded and continuous, we
     *  use an Array, which gives O(1) read/write access and takes memory
     *  proportional to the max possible number of free seats.
     *
     *  Each array link points to a min heap where we order tables by
     *  their total number of seats. So if 2 tables with 4 total seats and 6 total
     *  seats have currently 2 free seats and we need to assign a group of 2, we
     *  first assign the group to the first table with 4 total seats, so that we
     *  try to save the bigger table for later for a bigger group.
     *
     *  The heap gives O(log n) for insertion and removal of tables, where n is
     *  the number of tables in the heap. Worst case for n is M the total number
     *  of tables, but in reality this number is much lower because tables have
     *  different sizes. The access to the min element is O(1).
     *  
     *  Once we have removed a table from the array of priority queues, since 
     *  the table knows how many free seats it has (OccupiedTable class), we can
     *  easily reindex it in O(log n) time.
     *
     *  If we don't want to optimize the assignment of tables and try to save
     *  big tables, we can replace PriorityQueues with LinkedLists which gives
     *  faster O(1) insertion and removal of tables for assignement. Removing a 
     *  particular table is O(n).
     *
     *  To have both at the same time, we can index tables in a 2 dimensional 
     *  array of LinkedList, where one index gives the number of free seats and
     *  the second index gives the total number of seat. In that case, indexing
     *  a table is O(1), and retrieval of tables is O(n) worst, because we have
     *  to look up the second index starting from 1 until we find a table. We can 
     *  optimize this by keeping for every entries of the first index (number of
     *  free seats) an index which points to the first non-empty LinkedList with
     *  the tables that have the minimum number of total seats. This index can be 
     *  maintained every time we add or remove a table from the pool of tables.
     *  This way of organizing data would consume more memory, proportional to 
     *  N^2*M, where N is the max number of possible seats and M is the number
     *  of tables, because we have to create the 2 dimensional array for all
     *  possible pairs of index (N^2) and then we have to create M LinkedLists
     *  nodes to index the tables somewhere in that 2 dimensional array.
     *
     *  I am not sure of the "canonical" way to create this data structure in Java
     *  It looks like I can't create 2 dimensional arrays of generic type like
     *  LinkedList<E>[][]. Using directly something like
     *  ArrayList<ArrayList<LinkedList<OccupiedTable>>> is a bit cumbersome, so
     *  it would need to be wrapped away in some private class to clarify the code.
     */
    private ArrayList<PriorityQueue<OccupiedTable>> all_tables;

    /*
     *  First index in all_tables array where we can find free seats.
     *  It helps to quicken the look-up of tables with free seats.
     */
    private int min_free_seats;
    
    /**
     *  Last index in all_tables array where we can find free seats.
     *  It helps to quicken the look-up of tables with free seats.
     */
    private int max_free_seats;

    public SeatingManager(List<Table> tables) {
        int max_seats = -1;         // do not hardcode the max size of table
        for (Table t : tables) { if (max_seats < t.size) max_seats = t.size; }

        // init data structures
        all_groups = new HashMap<CustomerGroup,OccupiedTable>();
        waiting_groups = new LinkedList<CustomerGroup>();
        all_tables = new ArrayList<PriorityQueue<OccupiedTable>>(max_seats+1);
        for (int i = 0; i < this.all_tables.size(); i++) {
            all_tables.set(i, new PriorityQueue<OccupiedTable>());
        }


        // Index tables
        for (Table t : tables) {
            all_tables.get(t.size).add(new OccupiedTable(t));
        }

        // initiate range of free_seats
        this.max_free_seats = max_seats;
        this.min_free_seats = 1;
        update_min_free_seats();

    }


    /*
     *  Since we keep the group & table pairs in a map,
     *  we can query the table directly from the map in O(1).
     */
    public Table locate(CustomerGroup group) {
        return this.all_groups.get(group).table;
    }


    /*
     *  When a group arrives, we index it in the map all_group and try to locate
     *  a table with enough free seats (assign_seats()). If a table is not found
     *  we then add the group to the FIFO of waiting groups.
     */
    public void arrives(CustomerGroup group) {
        if (group == null) return;                  // guard against null input
        this.all_groups.put(group, null);           // the group has no table yet

        boolean was_seated = assign_seats(group);   // look for table, 
        if (!was_seated)
            this.waiting_groups.add(group);         // add to queue if no table, O(1)
    }


    /*
     *  When a group leaves, we must remove it from the map all_groups, in O(1).
     *  If that group did not have a table, we must also remove it from
     *  waiting_groups (worst O(K) where K is the number of waiting groups.
     *  Otherwise, we must update the status of the table, and try to assign a
     *  new group from the waiting_groups because the pool of free table has
     *  changed (worst runtime O(K.(N+log(M)) where N is the total number of free
     *  seats and M the total number of tables.
     */
    public void leaves(CustomerGroup group) {
        OccupiedTable table_to_free = this.all_groups.get(group);
        if( table_to_free != null) {
            all_tables.get(table_to_free.free_seats).remove(table_to_free);
            update_table_status(table_to_free, +group.size);
            for (CustomerGroup g : waiting_groups) {  // Try to assign groups
                if (g.size <= this.max_free_seats) {  // in waiting_groups. It is
                    assign_seats(g);                  // possible that several
                                                      // groups are assigned.
                }
            }
        } else {
            this.waiting_groups.remove(group);  // worst O(K)
        }
        this.all_groups.remove(group);          // remove from all_groups in O(1)
    }


    /*
     *  Try to find a table with enough free seats for a customer group.
     *  If a table is found, assign the table to this group and update the
     *  status of the table and its position in all_table.
     *  Return true if a table was found, otherwise false.
     *
     *  Run in O(N + log M) worst, where N is the max possible seats (6), and
     *  log M is the time to reindex the table in all_tables, with M the total
     *  number of tables.
     */
    private boolean assign_seats(CustomerGroup group) {
        OccupiedTable table_with_seats = this.find_table(group.size); // worst O(N)
        if(table_with_seats != null) {
            this.all_groups.put(group, table_with_seats);
            update_table_status(table_with_seats, -group.size); // O(log n)
            return true;
        }
        return false;
    }


    /*
     *  If the asked number of seats if bigger than max_free_seats, quickly
     *  return null.
     *
     *  If not determine if min_free_seats is higher than asked 
     *  number of seats. In that case we get a table in O(log n) where n is the 
     *  number of tables indexed with min_free_seats available. Then we update
     *  the min_free_seats if we took the last table with that number of free seats.
     *
     *  If we need more seats that the current min_free_seats, we look up 
     *  all_tables starting from the required number of seats, skipping indexes
     *  with empty sets of tables. Then we check if we need to update
     *  max_free_seats.
     *
     *  If a table is found, deindex it from all_tables. The caller
     *  has then the responsibility to reindex it. This is done by the method
     *  update_table_status().
     *
     *  In worst case scenario, this takes O(N) for looking up all_tables.
     */
    private OccupiedTable find_table(int number_of_seats) {
        if (number_of_seats > this.max_free_seats) return null; // quick fail

        OccupiedTable free_table;
        
        if (number_of_seats <= this.min_free_seats) {
            free_table = all_tables.get(this.min_free_seats).poll(); // in O(1)
            update_min_free_seats();                                 // worst O(N)
        } else {
            /* find first non-empty group of tables with enough room */
            while(all_tables.get(number_of_seats).isEmpty()) {    // worst O(N)
                number_of_seats++;
            }
            free_table = all_tables.get(number_of_seats).poll();  // in O(1)
            update_max_free_seats();                              // worst O(N)
        }

        return free_table;
    }


    /*
     *  Mute an OccupiedTable by updating its number of free_seats and reindex
     *  it in all_tables.
     *
     *  This also reindex the table in all_tables and updates accordingly 
     *  max_free_seats and min_free_seats
     *
     *  Runs in O(log n), because of insertion of table in PriorityQueue,
     *  where n is the expected number of table in the Priority Queue (worst is M)
     */
    private void update_table_status(OccupiedTable table, int size_difference) {
        table.free_seats += size_difference;
        this.all_tables.get(table.free_seats).add(table);
        if (table.free_seats < this.min_free_seats)
            this.min_free_seats = table.free_seats;
        if (table.free_seats > this.max_free_seats)
            this.max_free_seats = table.free_seats;
    }

    /* Runs in O(N) worst */
    private void update_min_free_seats() {
        while(this.all_tables.get(this.min_free_seats).size() == 0)
            this.min_free_seats++;
    }

    /* Runs in O(N) worst */
    private void update_max_free_seats() {
        while(this.all_tables.get(this.max_free_seats).size() == 0)
            this.max_free_seats--;
    }

}
