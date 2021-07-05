# ACLIB Converter module

Standalone converter module for [ACLIB](https://github.com/ACLIB/ACLIB). 
Decrypts and unpacks *.acd files.

> Work in progress.

Follow the [Documentation](https://github.com/ACLIB/Documentation) for more details.

# Example

```python
import aclib_converter as converter

# Folder test must exist.
converter.unpack('path_to_assettocorsa/content/cars/car_name/data.acd', './test')
```
