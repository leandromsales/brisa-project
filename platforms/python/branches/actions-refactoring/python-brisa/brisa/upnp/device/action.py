# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

from brisa.upnp.base_action import BaseAction, BaseArgument


class Argument(BaseArgument):

    IN = "in"
    OUT = "out"

    def __init__(self, arg_name, arg_direction, arg_state_var):
        """ Constructor for the Argument class.

        @param arg_name: argument name
        @param arg_direction: argument direction
        @param arg_state_var: argument related state variable

        @type arg_name: string
        @type arg_direction: string
        @type arg_state_var: string
        """
        BaseArgument.__init__(self, arg_name, arg_direction, arg_state_var)


class Action(BaseAction):

    def __init__(self, service, name, arguments = []):
        BaseAction.__init__(self, service, name, arguments)
        self.run_function = self.run

    def add_argument(self, argument):
        if argument:
            self.arguments.append(argument)

    def get_in_argument(self, name):
        for arg in self.arguments:
            if arg.direction == Argument.IN and arg.name == name:
                return arg
        return None

    def get_out_argument(self, name):
        for arg in self.arguments:
            if arg.direction == Argument.OUT and arg.name == name:
                return arg
        return None

    def __call__(self, *args, **kwargs):

        # Update in arguments
        in_kwargs = {}
        for arg_name, arg_value in kwargs.iteritems():
            arg = self.get_in_argument(arg_name)
            if not arg:
                #TODO raise an error - this arg not exists
                pass
            arg.state_var.update(arg_value)
            in_kwargs[arg_name] = arg_value

        out_args = self.run_function(*(), **in_kwargs)

        if not isinstance(out_args, dict):
            #TODO raise an error
            pass

        # Update out arguments
        return_args = {}
        for arg_name, arg_value in out_args.iteritems():
            arg = self.get_out_argument(arg_name)
            if not arg:
                #TODO raise an error - this arg not exists
                pass
            arg.state_var.update(arg_value)
            return_args[arg_name] = arg_value

        return {self.name + "Response": return_args}

    def run(self, *args, **kwargs):
        return {}
