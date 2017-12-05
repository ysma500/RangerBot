#include <libarmus.h>

int main()
{
	int test = 0;
  while (test == 0)
  {
    if (DIGITALIO_Read(1))
    {
    	LCD_ClearAndPrint("THIS IS BUMPER FRONT, BLEU entree 1");
    }
    if (DIGITALIO_Read(2))
    {
    	LCD_ClearAndPrint("THIS IS BUMPER REAR, ROUGE entree 2");
    }
    if (DIGITALIO_Read(3))
    {
    	LCD_ClearAndPrint("THIS IS BUMPER LEFT, ORANGE entree 3");
    }
	if (DIGITALIO_Read(4))
	{
		LCD_ClearAndPrint("THIS IS BUMPER RIGHT, VERT entree 4");
	}
	if (DIGITALIO_Read(11))
	{
		LCD_ClearAndPrint("THIS IS BUMPER ORANGE RIGHT, entree 17");
	}
	if (DIGITALIO_Read(9))
	{
		LCD_ClearAndPrint("THIS IS BUMPER ORAGNE LEFT, entree 19");
	}
  }
  return 0;
}
