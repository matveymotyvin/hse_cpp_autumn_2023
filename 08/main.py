#! /usr/bin/env python3

import json

import ujson

import cjson

from time import time

def test_perf(module, json_str, n=5):
    for i in range(n):
        module.dumps(module.loads(json_str))

def main():
    with open('data.json', 'r') as f:
        d = json.loads(f.read())
    jsn = "{"
    for key in d:
        jsn += '"' + key + '": "' + d[key] + '", '
    jsn = jsn[:-2] + "}"

    _ = time()
    test_perf(json, jsn)
    print(f"json time: {time()-_}s")
    _ = time()
    test_perf(ujson, jsn)
    print(f"ujson time: {time()-_}s")
    _ = time()
    test_perf(cjson, jsn)
    print(f"cjson time: {time()-_}s")
    
    #timeit.timeit('json.dumps(json.loads(json_str))', number=1000, globals=globals())
    #timeit.timeit('ujson.dumps(ujson.loads(json_str))', number=1000, globals=globals())
    #timeit.timeit('cjson.dumps(cjson.loads(json_str))', number=1000, globals=globals())

    strs = ['{"hello": 10, "world": "value"}', '{}', '{"hello": 10}', '{"world": "value"}',
            '{"hello": 10, "world": "value", "a": "uuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu"}',
            jsn]
    for json_str in strs:
        json_doc = json.loads(json_str)
        ujson_doc = ujson.loads(json_str)
        cjson_doc = cjson.loads(json_str)
        assert json_doc == ujson_doc == cjson_doc
        #print(json_str, cjson.loads(json_str), cjson.dumps(cjson.loads(json_str)))
        assert json_str == cjson.dumps(cjson.loads(json_str))

if __name__ == "__main__":
    main()
