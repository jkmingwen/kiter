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

architecture Behavioural of sbuffer is

begin
  assert (buffer_size >= 0) report "Shift register size must be positive" severity error;

    gen_sbuffer_n : if (buffer_size > 0) generate
        sbuff : entity work.sbuffer_n
            generic map (ram_width => ram_width,
                         buffer_size => buffer_size,
                         push_start => push_start,
                         pop_start => pop_start,
                         init => init)
            port map (clk => clk,
                      rst => rst,
                      cycle_count => cycle_count,
                      in_data => in_data,
                      out_data => out_data);
    end generate gen_sbuffer_n;

    gen_sbuffer_bypass : if (buffer_size = 0) generate
        sbuff : entity work.sbuffer_bypass
            generic map (ram_width => ram_width)
            port map (in_data => in_data,
                      out_data  => out_data);
    end generate gen_sbuffer_bypass;

end Behavioural;
