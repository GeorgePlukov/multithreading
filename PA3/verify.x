program VERIFYPROG {
	version VERIFYVERS {
		int RPCINITVERIFYSERVER(string) = 1;
		string RPCGetSeg(int) = 2;
	} = 1;
} = 0x12300002;