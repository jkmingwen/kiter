library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity sbuffer_t is
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
end entity sbuffer_t;

architecture Behavioural of sbuffer_t is

begin
  assert (buffer_size >= 0) report "Shift register size must be positive" severity error;

    gen_sbuffer_t_bypass : if (buffer_size = 0) generate
        sbuff : entity work.sbuffer_t_bypass
            generic map (ram_width => ram_width)
            port map (in_data => in_data,
                      out_data  => out_data);
    end generate gen_sbuffer_t_bypass;

    gen_sbuffer_t_one : if (buffer_size = 1 and init = 0) generate
        sbuff : entity work.sbuffer_t_one
            generic map (ram_width => ram_width)
            port map (clk => clk,
                      trigger_push => trigger_push,
                      in_data => in_data,
                      out_data => out_data);
    end generate gen_sbuffer_t_one;

    gen_sbuffer_t_n : if (buffer_size > 1 and init >= 0) generate
        sbuff : entity work.sbuffer_t_n
            generic map (ram_width => ram_width,
                         buffer_size => buffer_size,
                         init => init)
            port map (clk => clk,
                      rst => rst,
                      trigger_push => trigger_push,
                      trigger_pop => trigger_pop,
                      in_data => in_data,
                      out_data => out_data);
    end generate gen_sbuffer_t_n;

end Behavioural;
