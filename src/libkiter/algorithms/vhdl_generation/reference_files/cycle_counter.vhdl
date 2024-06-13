-- cycle_counter.vhd
-- Counts the cycles of the given clk signal at a given period
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity cycle_counter is
    generic (period  : integer := 0);
    port (
        clk         : in  std_logic;
        rst       : in  std_logic;
        count       : out integer range -1 to period-1 := 0
    );
end cycle_counter;

architecture Behavioral of cycle_counter is

    signal clock_count: integer range -1 to period-1 := 0;
begin

    process (clk)
    begin
        if rising_edge(clk) then
            clock_count <= clock_count + 1;
            if clock_count = period-1 then
                clock_count <= 0;
            end if;
        end if;
        if rst = '0' then
            clock_count <= -1;
        end if;
    end process;

    count <= clock_count;

end Behavioral;
