
#include "CapteurDeCouleur.h"

//Configuration du capteur de couleur
void Init_Colors(void)
{
	//Initialisation du capteur de couleur
	ERROR_CHECK(color_Init(adjd_dev));
	
	m_iCouleurDep = 0;

	cap_SetValue(CAP_RED, 15);
	cap_SetValue(CAP_GREEN, 15);
	cap_SetValue(CAP_BLUE, 15);
	cap_SetValue(CAP_CLEAR, 15);

	integrationTime_SetValue(INTEGRATION_RED, 255);
	integrationTime_SetValue(INTEGRATION_GREEN, 255);
	integrationTime_SetValue(INTEGRATION_BLUE, 255);
	integrationTime_SetValue(INTEGRATION_CLEAR, 255);
}

//Get current color function
int get_current_color()
{
	int color;
	int red, blue, green, clear;
	color_Read(red, blue, green, clear);
			//Red
			if (red >= (RED_R - LINE_HYST_PLUS) && red <= (RED_R + LINE_HYST_PLUS)
				&& blue >= (RED_B - LINE_HYST_PLUS) && blue <= (RED_B + LINE_HYST_PLUS)
				&& green >= (RED_G - LINE_HYST_PLUS) && green <= (RED_G + LINE_HYST_PLUS)
				&& clear >= (RED_C - LINE_HYST_PLUS) && clear <= (RED_C + LINE_HYST_PLUS))
			{
				color = START_RED;
			}
			//Grey
			else if (red >= (GREY_R - LINE_HYST) && red <= (GREY_R + LINE_HYST)
				&& blue >= (GREY_B - LINE_HYST) && blue <= (GREY_B + LINE_HYST)
				&& green >= (GREY_G - LINE_HYST) && green <= (GREY_G + LINE_HYST)
				&& clear >= (GREY_C - LINE_HYST) && clear <= (GREY_C + LINE_HYST))
			{
				color = START_GREY;
			}
			//Yellow
			else if (red >= (YELLOW_R - LINE_HYST_PLUS) && red <= (YELLOW_R + LINE_HYST_PLUS)
				&& blue >= (YELLOW_B - LINE_HYST_PLUS) && blue <= (YELLOW_B + LINE_HYST_PLUS)
				&& green >= (YELLOW_G - LINE_HYST_PLUS) && green <= (YELLOW_G + LINE_HYST_PLUS)
				&& clear >= (YELLOW_C - LINE_HYST_PLUS) && clear <= (YELLOW_C + LINE_HYST_PLUS))
			{
				color = START_YELLOW;
			}
			//Pink
			else if (red >= (PINK_R - LINE_HYST) && red <= (PINK_R + LINE_HYST)
				&& blue >= (PINK_B - LINE_HYST) && blue <= (PINK_B + LINE_HYST)
				&& green >= (PINK_G - LINE_HYST) && green <= (PINK_G + LINE_HYST)
				&& clear >= (PINK_C - LINE_HYST) && clear <= (PINK_C + LINE_HYST))
			{
				color = START_PINK;
			}
			//Green
			else if (red >= (GREEN_R - LINE_HYST) && red <= (GREEN_R + LINE_HYST)
				&& blue >= (GREEN_B - LINE_HYST) && blue <= (GREEN_B + LINE_HYST)
				&& green >= (GREEN_G - LINE_HYST) && green <= (GREEN_G + LINE_HYST)
				&& clear >= (GREEN_C - LINE_HYST) && clear <= (GREEN_C + LINE_HYST))
			{
				color = START_GREEN;
			}
			//Blue
			else if (red >= (BLUE_R - LINE_HYST) && red <= (BLUE_R + LINE_HYST)
				&& blue >= (BLUE_B - LINE_HYST) && blue <= (BLUE_B + LINE_HYST)
				&& green >= (BLUE_G - LINE_HYST) && green <= (BLUE_G + LINE_HYST)
				&& clear >= (BLUE_C - LINE_HYST) && clear <= (BLUE_C + LINE_HYST))
			{
				color = START_BLUE;
			}
			//White
			else if (red >= (WHITE_R - LINE_HYST) && red <= (WHITE_R + LINE_HYST)
				&& blue >= (WHITE_B - LINE_HYST) && blue <= (WHITE_B + LINE_HYST)
				&& green >= (WHITE_G - LINE_HYST) && green <= (WHITE_G + LINE_HYST)
				&& clear >= (WHITE_C - LINE_HYST) && clear <= (WHITE_C + LINE_HYST))
			{
				color = START_WHITE;
			}
			else
			{
				color = START_OTHER;
			}
	return color;
}