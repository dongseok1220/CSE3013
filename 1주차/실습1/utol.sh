echo "working directory:"
read dirname

if [ -n "$dirname" ]
then
        if [ -d $dirname ]
        then
                if [ -x $dirname ]
                then
                        cd $dirname
                else
                        echo "Permission denied"
                        exit 0
                fi
        else echo "No directory"
             exit 0
        fi
fi

for dir in *

do
if [ -f $dir ]
then
        newname=`echo $dir | tr "[a-z] [A-Z]" "[A-Z] [a-z]"` 
        mv $dir $newname 
fi

done

