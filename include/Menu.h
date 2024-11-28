#ifndef MENU_H
#define MENU_H

#include "RedSocial.h"
#include <vector>
#include <string>

void renderizarLogs(const std::vector<std::string>& logs);

void renderizarMenuPrincipal(RedSocial& red, bool& salirPrograma);

void renderizarMenuUsuario(RedSocial& red);

#endif // MENU_H
