

watchdog.out: wd_guard.cpp
	g++ wd_guard.cpp wd_api.a libwd_shared.so -lpthread -Wl,-rpath=. -o $@
