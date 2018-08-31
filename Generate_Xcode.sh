set -e
cd "`dirname "$0"`"

if [ -f Bulk/Tools/Scripts/Mac/Generate_Xcode.sh ]; then
    sh Bulk/Tools/Scripts/Mac/Generate_Xcode.sh
fi