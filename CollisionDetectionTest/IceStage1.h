/******************************************************************************
All content © 2017 Digipen Institute of Technology Singapore, all rights reserved

filename			IceStage1.h
author				Enriquez, Audry May Dela Paz(25%)| Wong Zhihao (25%)
					Ngm Hui Min, Lois (25%)			 | Ong Tze Ang (25%)
DP email			a.enriquez@digipen.edu			 | zhihao.wong@digipen.edu
					lois.ngm@digipen.edu			 | tzeang.ong@digipen.edu
course				BSGD							 | RTIS
					RTIS							 | RTIS

Brief Description:
Header file for IceStage1.c which contains the contents of the IceStage1 object
******************************************************************************/
#pragma once

/* necessary struct */
typedef struct Sound Sound;

/* Ice Stage 1 */
void iceStage_1(Sound *sound, int play_type, int terrain);