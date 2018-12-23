#pragma once

#ifndef NDEBUG

#define log(fmt, ...) printf(fmt, __VA_ARGS__)

#else

#define log(message) ((void)0)

#endif