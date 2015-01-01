#!/bin/bash

logfile=$HOME/Dropbox/current/logjournal

read logmsg

echo `date +"%F %T"` $logmsg >> $logfile
