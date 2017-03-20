typedef string str_t<255>;

struct my_struct {
    str_t data;
};

program VERIFYPROG {
	version VERIFYVERS {
		int RPCINITVERIFYSERVER() = 1;
		my_struct RPCGetSeg(int) = 2;
	} = 1;
} = 0x12340002;