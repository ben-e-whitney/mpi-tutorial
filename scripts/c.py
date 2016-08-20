import os

class Macro:
    def __init__(self, name, value):
        self.name = name
        self.value = value

    def __str__(self):
        return '#define {nam} {val}'.format(nam=self.name, val=self.value)

class Typedef:
    def __init__(self, type_specifier, declarator):
        self.type_specifier = type_specifier
        self.declarator = declarator

    def __str__(self):
        return 'typedef {spe} {dec};'.format(spe=self.type_specifier,
                                             dec=self.declarator)

class Enum:
    def __init__(self, name, values=None):
        self.name = 'enum {name}'.format(name=name)
        self.values = values or []

    def append(self, value):
        self.values.append(value)

    def __str__(self):
        return '{name} {{{vals}}};'.format(
            name=self.name,
            vals=', '.join(self.values)
        )

class Function:
    def __init__(self, name, argument_types, return_type, argument_names,
                 lines):
        self.name = name
        self.argument_types = argument_types
        self.return_type = return_type
        self.argument_names = argument_names
        assert len(self.argument_types) == len(self.argument_names)
        self.lines = lines

    def arguments(self, names=False):
        return self.argument_types if not names else tuple(map(
            lambda t, n: '{t} {n}'.format(t=t, n=n),
            self.argument_types,
            self.argument_names,
        ))

    def signature(self, names=False):
        return '{return_type} {name}({arglist}){end}'.format(
            return_type=self.return_type,
            name=self.name,
            arglist=', '.join(self.arguments(names=names)),
            end=';' if not names else ' {',
        )

    def __str__(self):
        return '\n'.join([
            self.signature(names=True),
            *self.lines,
            '}',
        ])

class Classifier(Function):
    def __init__(self, name, argument_type, return_type, pairings,
                 argument_name='x',
                 comparer=lambda a, b: '{a} == {b}'.format(a=a, b=b)):
        lines = [
            'if ({condition}) return {key};'.format(
                key=key,
                condition=' || '.join(
                    comparer(argument_name, value) for value in values
                ),
            ) for key, values in pairings.items()
        ]
        lines.extend([
            (
                'fprintf(stderr, "Unrecognized {arg_t} \'{flg}.\'\\n", {var});'
            ).format(
                arg_t=argument_type,
                #Good enough for now.
                flg='%d' if 'enum' in argument_type else '%s',
                var=argument_name
            ),
            'exit(1);',
        ])
        super().__init__(name, [argument_type], return_type, [argument_name],
                         lines)

class Header:
    def __init__(self, name):
        self.name = name

    def __str__(self):
        return '#include <{nam}>'.format(nam=self.name)

class IncludeGuard:
    def __init__(self, name):
        self.name = os.path.basename(name).replace('.', '_').upper()

    def begin(self):
        return '#ifndef {nam}\n#define {nam}'.format(nam=self.name)

    @staticmethod
    def end():
        return '#endif'

