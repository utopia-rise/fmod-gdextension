$1 -s --headless --path $PWD addons/gut/gut_cmdln.gd | ( no_error="false"; while read -r line
do
    echo "$line"
    if echo "$line" | grep -q "Passing tests     0"; then
        echo "ERROR: No tests passed !"
        exit 1
    fi
    if echo "$line" | grep -q "Failing tests     0"; then
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
