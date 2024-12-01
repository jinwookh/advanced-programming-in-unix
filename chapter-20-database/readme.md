# usage
```
gcc -o db.o db.c
```

automatically stores the records, which is defined in main function.   
example:   
```
if (db_store(db, "gamma", "record3", DB_INSERT) != 0)
		printf("db_store error for gamma");
```
above code stores record3 at .dat file, and stores key gamma and pointer of record at index file.(storing pointer value is not working this moment for reasons unknown)
