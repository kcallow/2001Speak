#include <pstreams/pstream.h>
#include <iostream>
using namespace std;

/* A simple wrapper over xte.
   Allows to control keyboard and mouse, and sleep.
*/
class XteWrapper {
	public:
		static void keypress(string key) {
			run("key", key);
		}
		static void keyup(string key) {
			run("keyup", key);
		}
		static void keydown(string key) {
			run("keydown", key);
		}
		static void str(string str) {
			run("str",str);
		}
		static void mouseclick(string buttonCode) {
			run("mouseclick", buttonCode);
		}
		static void mousemove(string x, string y) {
			run("mousemove", x + " " + y);
		}
		static void mousermove(string x, string y) {
			run("mousermove", x + " " + y);
		}
		static void mousedown(string buttonCode) {
			run("mousedown", buttonCode);
		}
		static void mouseup(string buttonCode) {
			run("mouseup", buttonCode);
		}
		static void sleep(string seconds) {
			run("sleep", seconds);
		}
		static void usleep(string microseconds) {
			run("usleep", microseconds);
		}

	private:
		static void run(string command, string params) {
			string exec = "xte '" + command + " " + params + "'";
			redi::ipstream proc(exec);
            proc.close();

		}

};

