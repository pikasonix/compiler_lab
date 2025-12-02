@echo off
echo Building project...
cd incompleted
make
if errorlevel 1 (
    echo Build failed.
    pause
    exit /b 1
)
cd ..

echo Running tests...
python run_tests.py
pause