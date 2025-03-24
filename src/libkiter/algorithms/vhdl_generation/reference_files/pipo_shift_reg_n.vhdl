library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity pipo_shift_reg_n is
  generic( depth : integer;
           period : integer;
           load : integer );
  port ( clk : in std_logic;
         rst : in std_logic;
         in_data : in std_logic_vector(33 downto 0);
         out_data : out std_logic_vector(33 downto 0) );
end pipo_shift_reg_n;

architecture Behavioral of pipo_shift_reg_n is
    type mem is array(depth-1 downto 0) of std_logic_vector(33 downto 0);
    signal bank : mem := (others => (others => '0'));
begin

    process (clk)
        variable clk_cnt : integer := 0;  --counter of master clock cycle
    begin
        if (rising_edge(clk)) then
            if (clk_cnt < period-1) then
                clk_cnt := clk_cnt + 1;
                if (clk_cnt = load) then
                    bank(depth-1) <= in_data;
                    bank(depth-2 downto 0) <= bank(depth-1 downto 1);
                end if;
            else
                clk_cnt := 0;
            end if;
        end if;
        if (rst = '0') then
            clk_cnt := -1;
        end if;
    end process;

    out_data <= bank(0);

end Behavioral;
