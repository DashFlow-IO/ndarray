#ifndef NDARRAY_EXPORT_H_
#define NDARRAY_EXPORT_H_

#if defined(WIN32)
#define NDARRAY_EXPORT __declspec(dllexport)
#else
#define NDARRAY_EXPORT __attribute__((visibility("default")))
#endif

#endif  //  NDARRAY_EXPORT_H_