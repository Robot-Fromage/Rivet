dir -Recurse *.cpp | Get-Content | Measure-Object -Line
dir -Recurse *.h | Get-Content | Measure-Object -Line