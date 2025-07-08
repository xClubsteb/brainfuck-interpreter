class BFInterpreter:
    """
    Brainfuck interpreter that runs Brainfuck code

    Use .brainfuck() method to run given Brainfuck code and get the output as string

    test = BFInterpreter("+++.")
    test_str = test.brainfuck() # runs the provided BF code and return it as str
    """

    def __init__(self, code):
        self.code = self._validate_code(code)
        self.memory = [0]*30000
        self.bracket_graph = self._create_graph(self.code)
        self.iPtr = 0# pointer for an index in code sequence
        self.ptr = 0 # memory pointer
        self.output_raw = []
        self.output = ''

    def brainfuck(self):
        self._run()
        self._transform_output()
        print(f'Output:"{self.output}"')
        return self.output
    
    def _run(self, max_steps=100_000, step=0):
        while self.iPtr < len(self.code):
            step += 1
            cmd = self.code[self.iPtr]

            if step >= max_steps:
                raise RuntimeError("Possible infinite loop")

            if cmd == '[':
                if self.memory[self.ptr] == 0:
                    self.iPtr = self.bracket_graph[self.iPtr]
                    continue
            elif cmd == ']':
                if self.memory[self.ptr] != 0:
                    self.iPtr = self.bracket_graph[self.iPtr]
                    continue

            elif cmd == '+':
                self.inc()
            elif cmd == '-':
                self.dec()
            elif cmd == '>':
                self.move_right()
            elif cmd == '<':
                self.move_left()
            elif cmd == '.':
                self.get_output()
            elif cmd == ',':
                self.get_input()
            self.iPtr += 1
        print('Program finished without errors')


    # Methods for each Brainfuck command (excluding "[" and "]")
    def inc(self):
        """increase memory cell by 1"""
        self.memory[self.ptr] = (self.memory[self.ptr] + 1) % 256

    def dec(self):
        """decrease memory cell by 1"""
        self.memory[self.ptr] = (self.memory[self.ptr] - 1) % 256

    def move_right(self):
        """move memory pointer to the right by 1"""
        self.ptr += 1
        if self.ptr >= len(self.memory):
            raise IndexError("Pointer moved out of memory: right")

    def move_left(self):
        """move memory pointer to the left by 1"""
        self.ptr -= 1
        if self.ptr < 0:
            raise IndexError('Pointer moved out out of memory: left')

    def get_output(self):
        """add current cell value to the output"""
        self.output_raw.append(self.memory[self.ptr])

    def get_input(self):
        """input ascii character to add in the memory cell"""
        inp = ord(input('input='))
        self.memory[self.ptr] = inp


    # helper functions
    def _transform_output(self):
        self.output = ''.join(chr(cell) for cell in self.output_raw)
    
    def _create_graph(self, code):
        """
        creates graph of each brackets index and add it in dictionary
        also adds it reverse in it

        +[>+].
        =>
        {1:4, 4:1}
        """
        stack = []
        brackets_graph = {}

        for i, command in enumerate(code):
            if command == '[':
                stack.append(i)
            elif command == ']':
                if not stack:
                    raise SyntaxError(f"Unmatched closing bracket at position {i}")
                last = stack.pop()
                brackets_graph[last] = i
                brackets_graph[i] = last
        if stack:
            raise SyntaxError(f"Unmatched opening bracket(s) at position(s): {stack}")
        return brackets_graph
    
    def _validate_code(self, code):
        new_code = ''
        for i, cmd in enumerate(code):
            if cmd not in '+-<>.,[]':
                raise SyntaxError(f"Invalid character '{cmd}' at position {i}")
            else:
                new_code += cmd
        return new_code