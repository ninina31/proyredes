/*		rpcentrega.x			*/

program RPCENTREGA_PROG {
	version RPCENTREGA_VERS {
		
		int ASKFORTIME(void)=1;
		
		int ASKFORSUPPLY(char*)=2;

		int AUTENTIFICARBOMBA(char*)=3;
		
		int CONFIRM(char*)=4;
		
	}=1;
}= 0x31111111;
