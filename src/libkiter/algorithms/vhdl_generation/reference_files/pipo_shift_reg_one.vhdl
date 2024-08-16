library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity pipo_shift_reg_one is
  generic ( period : integer;
            load : integer );
  port ( clk : in std_logic;
         rst : in std_logic;
         in_data : in std_logic_vector(33 downto 0);
         out_data : out std_logic_vector(33 downto 0) );
end pipo_shift_reg_one;

architecture Behavioral of pipo_shift_reg_one is
    signal bank : std_logic_vector(33 downto 0) := (others => '0');
begin

    process (clk)
        variable clk_cnt : integer := 0;
    begin
        if (rising_edge(clk)) then
            if (clk_cnt < period-1) then
                clk_cnt := clk_cnt + 1;
                if (clk_cnt = load) then
                    bank <= in_data;
                end if;
            else
                clk_cnt := 0;
            end if;
        end if;
        if (rst = '0') then
            clk_cnt := -1;
        end if;
    end process;

    out_data <= bank;

end Behavioral;
