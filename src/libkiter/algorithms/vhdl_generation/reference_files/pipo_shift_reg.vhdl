library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity pipo_shift_reg is
  generic( depth : integer;
           period : integer;
           load : integer );
  port ( clk : in std_logic;
         rst : in std_logic;
         in_data : in std_logic_vector(33 downto 0);
         out_data : out std_logic_vector(33 downto 0) );
end pipo_shift_reg;

architecture Behavioral of pipo_shift_reg is

begin
  assert (depth >= 0) report "Shift register size must be positive" severity error;
  assert (period > 0) report "Period must be positive" severity error;
  assert (load > 0) report "Load time must be positive" severity error;

  gen_pipo_zero : if (depth = 0) generate
    pipo : entity work.pipo_shift_reg_zero
        port map ( in_data => in_data,
                   out_data => out_data);
  end generate gen_pipo_zero;

  gen_pipo_one : if (depth = 1) generate
    pipo : entity work.pipo_shift_reg_one
        generic map (period => period,
                     load => load)
        port map (clk => clk,
                  rst => rst,
                  in_data => in_data,
                  out_data => out_data);
  end generate gen_pipo_one;

  gen_pipo_n : if (depth > 1) generate
    pipo : entity work.pipo_shift_reg_n
        generic map (depth => depth,
                     period => period,
                     load => load)
        port map (clk => clk,
                  rst => rst,
                  in_data => in_data,
                  out_data => out_data);
  end generate gen_pipo_n;

end Behavioral;
