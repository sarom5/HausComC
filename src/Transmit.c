#include "HausComC.h"

int trasmit(uint8_t datac, uint8_t *data)
{
#ifdef RASPBERRY
	set_GPIO_transmit();
	printf("Block RS485- BUS\n");
#endif

	int fd = open_port();
	if (fd > -1)
	{
		general_conf(fd);

		//calculating xor
		uint8_t Xor = xor(datac, data);
		printf("XOR-Generated: Wert = %d\n", Xor);

		uint8_t *XorData;
		XorData = data + datac + 1;
		*XorData = Xor;
		
		//put 255 before XOR
		XorData --;
		*XorData = 255;

		printf("\n\nTransmitting data\n");
		printf("_________________\n");

		//only Databytes (first one is the Address)
		//+1 for the Xor //+1 for End of Data
		int Data_len = datac - 1 + 2;
		uint8_t Address[1] = { *data };
		uint8_t Stop[1] = { 255 };
		data++;
		printf("Databytes : %i  \n", Data_len);
		uint8_t transmit[Data_len];
		
		
#ifdef RASPBERRY
		set_GPIO_transmit();
		printf("Set RS485- BUS to Transmission\n");
#endif

		for (int i = 1; i < Data_len + 1; i++)
		{
			transmit[i - 1] = *data;

			data++;
		}
		//setSPACE(fd);
		setMARK(fd);
		int n = write(fd, Address, 1);
		if (n < 0)
		{
			perror("write() of Address failed!\n");
			return -1;
		}
		else
		{
			printf("write address %i\n", Address[0]);
		}

		//setMARK();
		setSPACE(fd);
		n = write(fd, transmit, Data_len);
		if (n < 0)
		{
			perror("write() of Data failed!\n");
			return -1;
		}
		else
		{
			printf("write data: ");
			for (uint8_t i = 0; i < Data_len; i++)
				printf(" %d,", transmit[i]);
		}
		printf(" \n");

		setMARK(fd);//
		//setSPACE(fd);
		n = write(fd, Stop, 1);
		if (n < 0)
		{
			perror("write() of Stop failed!\n");
			return -1;
		}
		else
			printf("write stop \n");

		close(fd);

		printf("\n\nTransmission complete! \nThanks for using HausCom2.\n");
		printf("If you need help, please insert \"-h\" as argument.\n\n\n");
		

		return 0;
	}
	else
		return -1;
}