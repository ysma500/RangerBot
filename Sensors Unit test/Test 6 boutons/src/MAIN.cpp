#include <libarmus.h>

int main()
{
	int test = 0;
  while (test == 0)
  {
    if (DIGITALIO_Read(BMP_FRONT))
    {
    	LCD_ClearAndPrint("THIS IS BUMPER FRONT, entree 1");
    }
    if (DIGITALIO_Read(BMP_REAR))
    {
    	LCD_ClearAndPrint("THIS IS BUMPER REAR, entree 2");
    }
    if (DIGITALIO_Read(BMP_LEFT))
    {
    	LCD_ClearAndPrint("THIS IS BUMPER LEFT, entree 3");
    }
	if (DIGITALIO_Read(BMP_RIGHT))
	{
		LCD_ClearAndPrint("THIS IS BUMPER RIGHT, entree 4");
	}
	if (DIGITALIO_Read(9))
	{
		LCD_ClearAndPrint("THIS IS BUMPER ORANGE RIGHT, entree 17");
	}
	if (DIGITALIO_Read(11))
	{
		LCD_ClearAndPrint("THIS IS BUMPER ORAGNE LEFT, entree 19");
	}
  }
  return 0;
}
