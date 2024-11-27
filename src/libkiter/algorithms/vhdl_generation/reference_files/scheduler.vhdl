-- scheduler.vhdl
-- Counts the cycles of the given clk signal at a given period
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

$ENTITY_DECLARATION

architecture Behavioral of scheduler is

    signal clock_count : integer;
begin

    reset_process :  process (clk)
    begin
        if rising_edge(clk) then
            if rst = '0' then
                trigger <= (others => '0');
            end if;
        end if;
    end process reset_process;

    execute_schedule : process (cycle_count)
    begin
        case (cycle_count) is
            $PROCESS_BEHAVIOUR
            when others =>
                null;
        end case;
    end process execute_schedule;

end Behavioral;
