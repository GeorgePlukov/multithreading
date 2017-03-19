program VERIFYPROG {
	version VERIFYVERS {
		int RPCINITVERIFYSERVER(string) = 1;
		int RPCGetSeg(string) = 2;
	} = 1;
} = 0x12300002;