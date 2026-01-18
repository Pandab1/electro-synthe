#ifndef GUI_COMPONENTS_H
#define GUI_COMPONENTS_H
#include "raylib.h"
#include <stdbool.h>

void DessinerSliderAvecBoutons(Rectangle zoneGroupe,
                               const char *titre,
                               float *valeur,
                               float min, float max,
                               float pas,
                               const char *formatTexte);
bool DessinerBoutonOnde(Rectangle zone, int iconId,
                        const char *libelle, bool selectionne);
#endif
