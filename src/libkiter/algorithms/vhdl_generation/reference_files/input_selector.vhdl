-- Takes in multiple inputs and routes them to the output port depending
-- on the phase of execution defined by 'current_phase'.
library ieee;
use ieee.std_logic_1164.all;

$ENTITY_DECLARATION

architecture Behavioral of $COMPONENT_NAME is
    signal current_phase, next_phase : integer := 0;

begin

  update_execution_phase : process(clk)
  begin
    if falling_edge(clk) then
        -- check out_ready_0 for both inputs as it corresponds to
        -- the input port for the AXI merger component
        -- check for valid signal on input port corresponding to execution phase

        $PROCESS_BEHAVIOUR

        -- reset behaviour
        if (rst = '0') then
            current_phase <= 0;
            next_phase <= 0;
        end if;
    end if;

    if rising_edge(clk) then
        current_phase <= next_phase;
    end if;
  end process update_execution_phase;

  $VALID_SIGNAL_ROUTING
  $DATA_SIGNAL_ROUTING
  $READY_SIGNAL_ROUTING

end Behavioral;
