default: js swf

js:
	make -f Makefile.js

swf:
	make -f Makefile.swf

clean:
	make -f Makefile.js clean
	make -f Makefile.swf clean
