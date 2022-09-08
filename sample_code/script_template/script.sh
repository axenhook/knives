#!/bin/bash

# ���ű����ɵ���־
LOG_DIR=.
LOG_NAME=$2.log
LOG_FULL_NAME=$LOG_DIR/$LOG_NAME

# ��������־�ļ�
rename_log()
{
	timeStamp=`date +"%04Y%02m%02d_%02k%02M%02S"`
	if [ -f $1/$2 ]; then
		#echo mv $1/$2 $1/$timeStamp"_$2"
		mv $1/$2 $1/$timeStamp"_$2"
	fi
}

# ��ʼ�ű�
script_start()
{
	while [ 1 ]; do
		echo -n [`date +"%04Y-%02m-%02d %02k:%02M:%02S"`]" "
		echo script is running...
		
		sleep 1
	done
}

# ֹͣ�ű�
script_stop()
{
	ps -ef | grep $0 | grep -v "grep\|stop" | awk {'print $2'} | xargs kill -9 >/dev/null 1>/dev/null 2>/dev/null
}

# ��ѯ�ű��Ƿ��Ѿ�������
script_status()
{
	ps -ef | grep $0 | grep -v "grep\|status"
}

# ������
main()
{
	case "$1" in
		start)
			# ������֮ǰ����־�ļ�
			rename_log $LOG_DIR $LOG_NAME
			
			script_start >$LOG_FULL_NAME 1>$LOG_FULL_NAME 2>$LOG_FULL_NAME &
			;;
			
		status)
			script_status
			;;
		
		stop)
			script_stop
			;;
			
		*)
			echo "usage: $0 start|status|stop"
			;;
	esac
}

main $*
