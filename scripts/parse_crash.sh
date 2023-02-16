#!/bin/bash

#echo "Hello Crash !"
#echo "Current dir: $0"
#echo "Program installation dir: $1"
#echo "Crash information file name: $2"

function checkFileIfNotExit(){
	if [ ! -e "$1" ];then
		echo "$1"" Does not exist. Please confirm the existence of the file and try again!"
		exit
	fi
}

SYMBOLS="symbols/"

PROGRAM_NAME="rec_app_node"
PROGRAM_PATH="$1"$PROGRAM_NAME

checkFileIfNotExit "$PROGRAM_PATH"

CRASH_FILE_DIR="$HOME/app_dump/"
CRASH_FILE_NAME="$2"
CRASH_FILE_PATH="$CRASH_FILE_DIR$CRASH_FILE_NAME"
CRASH_RESULT_PATH="$CRASH_FILE_DIR${CRASH_FILE_NAME%.*}.txt"
CRASH_SYMBOLS_PATH="$CRASH_FILE_DIR$SYMBOLS"
checkFileIfNotExit "$CRASH_FILE_PATH"

APP_SYM_FILE=$CRASH_FILE_DIR$PROGRAM_NAME".sym"

#echo "Full path of binary file: $PROGRAM_PATH"
#echo "Full path of sym file: $APP_SYM_FILE"
dump_syms "$PROGRAM_PATH" > "$APP_SYM_FILE"

headN1=`head -n1 "$APP_SYM_FILE"`
echo "First line of sym file: $headN1"

#取head1中被空格分割的第四个参数
symbol_num=`echo $headN1 | awk '{print $4}'`
#echo "Get the fourth string: $symbol_num"

symbols_dir=$CRASH_FILE_DIR$SYMBOLS$PROGRAM_NAME"/"$symbol_num
#echo "symbols_dir: $symbols_dir"
mkdir -p "$symbols_dir"
mv "$APP_SYM_FILE" "$symbols_dir"

#echo "$CRASH_FILE_PATH"
#echo "$CRASH_SYMBOLS_PATH"
#echo "$CRASH_RESULT_PATH"
minidump_stackwalk $CRASH_FILE_PATH $CRASH_SYMBOLS_PATH > $CRASH_RESULT_PATH

rm -rf "$CRASH_SYMBOLS_PATH"

exit