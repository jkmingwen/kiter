-- Input selector (scheduled)
-- Takes in N inputs, and produces 1 output
-- At each exec_time_N, the input data on the respective port is written to output,
-- this value will not change until the next push time occurs

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

$ENTITY_DECLARATION

architecture Behavioral of $COMPONENT_NAME is

begin

    select_input :  process (clk)
    begin
        if rising_edge(clk) then
            case (cycle_count) is
                $PROCESS_BEHAVIOUR
                when others =>
                    null;
            end case;
        end if;
    end process select_input;


end Behavioral;
