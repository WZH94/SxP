/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename			FireStage2.h
author				Enriquez, Audry May Dela Paz(25%)| Wong Zhihao (25%)
					Ngm Hui Min, Lois (25%)			 | Ong Tze Ang (25%)
DP email			a.enriquez@digipen.edu			 | zhihao.wong@digipen.edu
					lois.ngm@digipen.edu			 | tzeang.ong@digipen.edu
course				BSGD							 | RTIS
					RTIS							 | RTIS

Brief Description:
Header file for FireStage2.c which contains the contents of the FireStage2 object
******************************************************************************/
#pragma once
/* necessary struct */
typedef struct Sound Sound;

/* Fire Stage 2 */
void fireStage_2(Sound *sound, int play_type, int terrain);