program APPENDPROG {
	version APPENDVERS {
		int RPCINITAPPENDSERVER(string) = 1;
		int RPCAPPEND(string) = 2;
	} = 1;
} = 0x12340001;