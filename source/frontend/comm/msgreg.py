# Handlers -------------------------------------------------------------------------------------------------
# All messages begin with a handler keyword
CRUD_HANDLER = "CRUD "
ACTION_HANDLER = "ACTN "

# Actions --------------------------------------------------------------------------------------------------
TOGGLE_AUDIO = "TOGGLE_AUDIO"

# CRUD -----------------------------------------------------------------------------------------------------
# Standard CRUD messages fit the format {Operation}:{Module}:{NodeID}|{Param1},{Param2},...
CR_OP = "C:"
RD_OP = "R:"
UP_OP = "U:"
DL_OP = "D:"

# Module Names ---------------------------------------------------------------------------------------------
# Special
OUT = "Out:"        # {Operation}:Out:{NodeID}
CHAIN = "Chain:"    # {Operation}:Chain:{NodeID}|{ElementNodeID1},{ElementNodeID2},...

# Generators
SIMPLE_OSC = "SimpleOsc:"
BEEPER = "Beeper:"

# Effectors
SIMPLE_REVERB = "SimpleReverb:"
DELAY = "Delay:"
GAIN = "Gain:"

# Modulators

# Error Handling -------------------------------------------------------------------------------------------
INVALID_MODULE = ""
