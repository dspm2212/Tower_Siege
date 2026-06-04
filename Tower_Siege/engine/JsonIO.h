/*
 * Variant:  Tower Siege
 * Student:  Daniel Santiago Perez Madera - 20231020203
 *
 * JsonIO.h
 * --------
 * Hand-rolled JSON parser and serialiser for the Tower Siege engine.
 * No external JSON libraries used (hard constraint -- stdlib only).
 *
 * Schema (see data/ for full examples):
 *   input.json   (Python -> C++)  -- parsed into GameInput
 *   state.json   (C++ -> Python)  -- serialised from GameState
 */

#ifndef TOWER_SIEGE_JSON_IO_H
#define TOWER_SIEGE_JSON_IO_H

#include "GameTypes.h"
#include <string>

/* Read the entire contents of `path` into `out`.
   Returns true on success, false if the file cannot be opened. */
bool read_file(const std::string &path, std::string &out);

/* Write `content` to `path` (overwrites if it exists).
   Returns true on success. */
bool write_file(const std::string &path, const std::string &content);

/* Parse the JSON text in `text` into `input`.
   Returns true if all required fields were found. */
bool parse_input(const std::string &text, GameInput &input);

/* Read and parse input.json in one call. */
bool read_input(const std::string &path, GameInput &input);

/* Serialise `state` to a pretty-printed JSON string. */
std::string serialise_state(const GameState &state);

/* Serialise and write state.json in one call. */
bool write_state(const std::string &path, const GameState &state);

#endif /* TOWER_SIEGE_JSON_IO_H */
