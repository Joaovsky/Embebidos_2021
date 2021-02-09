PY=python -m py_compile
.PHONY:
    all
    test
    install
    compile
all:
    @+make test
    @make install
test:
    nosetest
install:
    python setup.py\
    install
compile:
    $(PY) app.py
circle:
    # of life
    circle
empty:
    # this is a comment
