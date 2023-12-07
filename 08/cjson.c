#include <stdio.h>
#include <stdlib.h>
#include <Python.h>

char* substr(const char* inpStr, int startPos, int strLen) {
	/* Cannot do anything with NULL. */

	if (inpStr == NULL) return NULL;

	/* All negative positions to go from end, and cannot
	   start before start of string, force to start. */

	if (startPos < 0)
		startPos = strlen(inpStr) + startPos;
	if (startPos < 0)
		startPos = 0;

	/* Force negative lengths to zero and cannot
	   start after end of string, force to end. */

	if (strLen < 0)
		strLen = 0;
	if (startPos > strlen(inpStr))
		startPos = strlen(inpStr);

	/* Adjust length if source string too short. */

	if (strLen > strlen(&inpStr[startPos]))
		strLen = strlen(&inpStr[startPos]);

	/* Get long enough string from heap, return NULL if no go. */

	char* buff = malloc(strLen + 1);

	/* Transfer string section and return it. */

	memcpy(buff, &(inpStr[startPos]), strLen);
	buff[strLen] = '\0';

	return buff;
}

int find(char* str, int size, int offset, char* token)
{
	int token_len = strlen(token);
	for (int i = offset; i < size - token_len; i++)
	{
		int found = 1;
		for (int j = i; (j < i + token_len) && found; j++) if (str[j] != token[j - i]) found = 0;
		if (found) return i;
	}
	return size;
}

int isnumber(char* str)
{
	int size = strlen(str);
	for (int i = 0; i < size; i++) if (str[i] < '0' || (str[i] - '0')>9) return 0;
	return 1;
}

int stoi(char* str)
{
	int size = strlen(str);
	int a = 0;
	for (int i = 0; i < size; i++) a = a * 10 + (str[i] - '0');
	return a;
}

PyObject* cjson_loads(PyObject* self, PyObject* args)
{
	char* str;
	int size;

	if (!PyArg_ParseTuple(args, "s#", &str, &size)) return NULL;

	PyObject* dict = NULL;
	if (!(dict = PyDict_New())) {
		printf("ERROR: Failed to create Dict Object\n");
		return NULL;
	}
	char* token1 = "\"", * token2 = ",";
	for (int offset = 0; offset < size;)
	{
		offset = find(str, size, offset, token1);
		if (offset == size) break;
		int key_end = find(str, size, offset + 1, token1);
		int val_start = key_end + 2;
		int val_end = find(str, size, val_start + 1, token2);
		val_end = val_end < (size - 1) ? val_end : (size - 1);

		int key_len = key_end - offset - 1, val_len = val_end - val_start - 1;
		char* key_ = substr(str, offset + 1, key_len);
		char* val_ = substr(str, val_start + 1, val_len);

		PyObject* key = NULL;
		PyObject* value = NULL;
		if (isnumber(val_) && \
			(key = Py_BuildValue("s", key_)) && \
			(value = Py_BuildValue("i", stoi(val_))))
		{
			if (PyDict_SetItem(dict, key, value) < 0) {
				printf("ERROR: Failed to set item\n");
				return NULL;
			}
		}
		else if ((key = Py_BuildValue("s", key_)) && \
			(value = Py_BuildValue("s", substr(val_, 1, val_len - 2))))
		{
			if (PyDict_SetItem(dict, key, value) < 0) {
				printf("ERROR: Failed to set item\n");
				return NULL;
			}
		}

		offset = val_end + 1;
	}

	PyErr_Clear();

	return dict;
}

struct varstr
{
	char* str;
	int capacity;
	int size;
};

void append_string(struct varstr* str, char* other)
{
	int size = strlen(other);
	if (str->size + size >= str->capacity)
	{
		char* newstr = malloc(str->size + size + 5);
		strcpy(newstr, str->str);
		strcat(newstr, other);
		free(str->str);
		str->str = newstr;
		str->capacity = str->size + size + 5;
		str->size = str->capacity;
	}
	else
	{
		strcat(str->str, other);
		str->size += size;
	}
}

PyObject* cjson_dumps(PyObject* self, PyObject* args)
{
	PyObject* dict = args;

	if (!PyArg_ParseTuple(args, "O", &dict))
	{
		printf("ERROR: Failed to parse arguments\n");
		return NULL;
	}

	struct varstr str;
	str.str = malloc((1<<16)); strcpy(str.str, "{"); str.size = 1; str.capacity = (1 << 16);
	int bufsize = 0, bufi = 0;
	char* bufs = "";

	PyObject* key = NULL, * value = NULL;
	Py_ssize_t ppos = 0;
	int cnt = 0;

	//printf("%d\n", PyDict_CheckExact(dict));

	while (PyDict_Next(dict, &ppos, &key, &value))
	{
		if (!PyArg_Parse(key, "s", &bufs)) return NULL;
		if (cnt++) append_string(&str, ", ");
		append_string(&str, "\"");
		append_string(&str, bufs);
		append_string(&str, "\": ");
		//printf("%d\n", str.size);
		if (!PyArg_Parse(value, "i", &bufi))
		{
			if (!PyArg_Parse(value, "s", &bufs)) return NULL;
			append_string(&str, "\"");
			append_string(&str, bufs);
			append_string(&str, "\"");
		}
		else
		{
			sprintf(bufs, "%d", bufi);
			append_string(&str, bufs);
			//append_string(&str, ", ", 2);
		}
	}

	append_string(&str, "}");
	//printf("%s\n", str.str);

	PyErr_Clear();

	return Py_BuildValue("s", str.str);
}

static PyMethodDef methods[] = {
	{"loads", cjson_loads, METH_VARARGS, "load json from string, using C API"},
	{"dumps", cjson_dumps, METH_VARARGS, "dump json to string, using C API"},
	{NULL, NULL, 0, NULL}
};

static struct PyModuleDef cjsonmodule = {
	PyModuleDef_HEAD_INIT, "cjson",
	NULL, -1, methods
};

PyMODINIT_FUNC PyInit_cjson(void)
{
	return PyModule_Create(&cjsonmodule);
}