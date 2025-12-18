TEST:
foreach ($i in 1..6) { echo "Testing example$i..."; .\kplc.exe "..\tests\example$i.kpl" > "..\output\example$i.out" 2>&1; echo "Exit code: $LASTEXITCODE`n" }