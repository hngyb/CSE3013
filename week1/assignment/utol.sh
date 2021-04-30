echo "Working directory"
read dirname

if [ -n "$dirname" ]
then    
        if [ -d "$dirname" ]
        then
                cd $dirname
                if [ $? != 0 ]
                then
                        echo "cd $dirname failed"
                        exit 1
                fi
        else
                echo "No $dirname directory exits"
        fi
fi

for dir in *
do
        newname=`echo $dir | tr "[a-z] [A-Z]" "[A-Z] [a-z]"`
        mv $dir $newname
done
