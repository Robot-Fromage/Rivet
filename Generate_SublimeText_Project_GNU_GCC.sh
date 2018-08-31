set -e
cd "`dirname "$0"`"

if [ -f Bulk/Tools/Scripts/Linux/Generate_SublimeText_Project_GNU_GCC.sh ]; then
    sh Bulk/Tools/Scripts/Linux/Generate_SublimeText_Project_GNU_GCC.sh
fi