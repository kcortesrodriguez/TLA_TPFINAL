./osocompiler < $1
mkdir -p output
mv output.java output 2>/dev/null #Silent mv
cd output

if [ -f output.java ]; then
javac -g output.java
java -classpath : output
fi