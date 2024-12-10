library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity sbuffer is
    generic (ram_width : integer;
             buffer_size : integer;
             init : integer);
    port (
        clk         : in  std_logic;
        rst       : in  std_logic;
        trigger_push : in std_logic;
        trigger_pop : in std_logic;
        in_data   : in  std_logic_vector(ram_width-1 downto 0);
        out_data    : out std_logic_vector(ram_width-1 downto 0));
end entity sbuffer;

architecture Behavioural of sbuffer is

begin
  assert (buffer_size >= 0) report "Shift register size must be positive" severity error;

    gen_sbuffer_bypass : if (buffer_size = 0) generate
        sbuff : entity work.sbuffer_bypass
            generic map (ram_width => ram_width)
            port map (in_data => in_data,
                      out_data  => out_data);
    end generate gen_sbuffer_bypass;

    gen_sbuffer_one : if (buffer_size = 1 and init = 0) generate
        sbuff : entity work.sbuffer_one
            generic map (ram_width => ram_width)
            port map (clk => clk,
                      trigger_push => trigger_push,
                      in_data => in_data,
                      out_data => out_data);
    end generate gen_sbuffer_one;

    gen_sbuffer_n : if (buffer_size > 1 and init >= 0) generate
        sbuff : entity work.sbuffer_n
            generic map (ram_width => ram_width,
                         buffer_size => buffer_size,
                         init => init)
            port map (clk => clk,
                      rst => rst,
                      trigger_push => trigger_push,
                      trigger_pop => trigger_pop,
                      in_data => in_data,
                      out_data => out_data);
    end generate gen_sbuffer_n;

end Behavioural;
