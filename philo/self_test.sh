for i in {1..50}; do
	./philo 199 610 200 200 5 > log.txt || echo "FAILED run $i"
	grep died log.txt && echo "DIED run $i"
done