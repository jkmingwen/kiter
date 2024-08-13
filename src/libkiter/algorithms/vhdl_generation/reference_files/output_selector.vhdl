-- Takes in two inputs, sending to output at the
-- same time. Will store input if they don't arrive at the same time; will only
-- output once both inputs are provided.
library ieee;
use ieee.std_logic_1164.all;

$ENTITY_DECLARATION

architecture Behavioral of $COMPONENT_NAME is
    signal current_phase, next_phase : integer := 0;


begin

  update_execution_phase : process(clk)
  begin
    if falling_edge(clk) then

        $PROCESS_BEHAVIOUR

        -- rst behaviour
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
