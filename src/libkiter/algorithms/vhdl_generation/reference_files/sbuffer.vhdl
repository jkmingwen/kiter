-- Scheduled buffer
-- Time-driven buffer: stores data from in_data to memory at given push_start
-- time; writes data from memory at given pop_start time.
-- Operates on a FIFO basis, with the possibility of having N<buffer_size-1
-- initial null (0) values. Write and read indexes advance after each push/pop
-- operation.
-- Requires an input of clock cycle counts (we use cycle_counter.vhd) to function.
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity sbuffer is
    generic (ram_width : integer;
             buffer_size : integer;
             push_start : integer; -- start time of write operation
             pop_start : integer; -- start time of read operation
             init : integer);
    port (
        clk         : in  std_logic;
        rst       : in  std_logic;
        cycle_count : in integer;
        in_data   : in  std_logic_vector(ram_width-1 downto 0);
        out_data    : out std_logic_vector(ram_width-1 downto 0));
end entity sbuffer;

architecture Behavioral of sbuffer is
    type mem_type is array (0 to buffer_size-1) of std_logic_vector(ram_width-1 downto 0);
    signal memory : mem_type := (others => (others => '0'));
    signal write_index : integer range 0 to buffer_size-1 := init;
    signal read_index : integer range 0 to buffer_size-1 := 0;
begin

    push_operation :  process (clk)
    begin
        if rising_edge(clk) then
            case (cycle_count) is
                when push_start =>
                    memory(write_index) <= in_data;
                    write_index <= (write_index + 1) mod buffer_size;
                when others =>
                    null;
            end case;
        end if;
        if (reset = '0') then
            write_index <= init;
            memory <= (others => (others => '0'));
        end if;
    end process push_operation;

    pop_operation :  process (clk)
    begin
        if rising_edge(clk) then
            case (cycle_count) is
                when pop_start =>
                    out_data <= memory(read_index);
                    read_index <= (read_index + 1) mod buffer_size;
                when others =>
                    null;
            end case;
        end if;
        if (reset = '0') then
            read_index <= 0;
        end if;
    end process pop_operation;

end architecture Behavioral;
