-- Scheduled buffer one
-- Acts as sample and hold: sets out_data to in_data at given push_start time.
-- Requires an input of clock cycle counts (we use cycle_counter.vhd) to function.
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity sbuffer_one is
    generic (ram_width : integer;
             buffer_size : integer;
             push_start : integer; -- start time of write operation
             init : integer);
    port (
        clk         : in  std_logic;
        cycle_count : in integer;
        in_data   : in  std_logic_vector(ram_width-1 downto 0);
        out_data    : out std_logic_vector(ram_width-1 downto 0));
end entity sbuffer_one;

architecture Behavioral of sbuffer_one is
begin

    push_operation :  process (clk)
    begin
        if rising_edge(clk) then
            if (cycle_count = push_start) then
                out_data <= in_data;
            end if;
        end if;
    end process push_operation;

end architecture Behavioral;
