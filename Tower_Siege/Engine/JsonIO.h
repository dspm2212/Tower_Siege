
/*
 JsonIO.h
 -------
 Funciones auxiliares para leer y escribir los JSON de entrada y estado
 (input.json / state.json). Implementación minimalista y sin dependencias
 externas; adecuada para la estructura de datos controlada de la práctica.
*/
#ifndef TOWER_SIEGE_JSONIO_H
#define TOWER_SIEGE_JSONIO_H

#include "GameTypes.h"
#include <string>
#include <vector>

/* Lee todo el texto del archivo `path` en `output`. */
bool readFileText(const std::string& path, std::string& output);

/* Escribe `content` en `path` (sobrescribe si existe). */
bool writeFileText(const std::string& path, const std::string& content);

/* Parsea el contenido JSON de `text` y rellena `input`.
	Retorna true si el parseo fue exitoso.
*/
bool parseGameInput(const std::string& text, GameInput& input);

/* Lee y parsea `path` en una sola llamada. */
bool readGameInputFromFile(const std::string& path, GameInput& input);

/* Serializa `GameState` a texto JSON con formato legible. */
std::string serializeGameState(const GameState& state);

/* Escribe el estado serializado en `path`. */
bool writeGameStateToFile(const std::string& path, const GameState& state);

#endif // TOWER_SIEGE_JSONIO_H
