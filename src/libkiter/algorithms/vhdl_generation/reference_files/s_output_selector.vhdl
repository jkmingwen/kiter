-- Output selector (scheduled)
-- Takes in 1 input, and produces data on one of N output ports
-- At each push time, the data on the input port is written to memory
-- At each exec_time_N, the next datum stored in memory is written to the respective output port

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

$ENTITY_DECLARATION

architecture Behavioral of $COMPONENT_NAME is

begin

    send_output :  process (clk)
    begin
        if rising_edge(clk) then
            case (cycle_count) is
                $PROCESS_BEHAVIOUR
                when others =>
                    null;
            end case;
        end if;
    end process send_output;

end Behavioral;
