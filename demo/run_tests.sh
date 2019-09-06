$1 -d -s --path $PWD addons/gut/gut_cmdln.gd | ( no_error="false"; while read -r line
do
    echo "$line"
    if echo "$line" | grep -q "failing asserts:  0"; then
        no_error="true"
    fi
done
if [[ "$no_error" == "true" ]]; then
    exit 0
else
    echo "ERROR: Some assertions failed !"
    exit 1
fi
); exit $?
