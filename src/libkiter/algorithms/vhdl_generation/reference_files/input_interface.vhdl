library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity input_interface is
    generic (
        i2s_bit_width : natural := 24;
        fp_bit_width : natural := 34;
        bit_depth : natural := 2);
    port (
        clk : in std_logic;
        ws : in std_logic;
        rst : in std_logic;
        l_valid : out std_logic := '0';
        r_valid : out std_logic := '0';
        l_ready : in std_logic;
        r_ready : in std_logic;
        l_data_in : in std_logic_vector(i2s_bit_width - 1 downto 0);
        r_data_in : in std_logic_vector(i2s_bit_width - 1 downto 0);
        l_data_out : out std_logic_vector(i2s_bit_width - 1 downto 0) := (others => '0');
        r_data_out : out std_logic_vector(i2s_bit_width - 1 downto 0) := (others => '0') );
end input_interface;

architecture Behavioral of input_interface is
    signal l_data_store : std_logic_vector(i2s_bit_width - 1 downto 0) := (others => '0');
    signal r_data_store : std_logic_vector(i2s_bit_width - 1 downto 0)  := (others => '0');
    signal l_stored : std_logic := '0';
    signal r_stored : std_logic := '0';
    signal l_sent : std_logic := '0';
    signal r_sent : std_logic := '0';

begin

    store_data_l : process(ws, clk, rst)
    begin
        if (rst = '0') then
            l_stored <= '0';
            l_data_store <= (others => '0');
        elsif falling_edge(ws) then
            if (l_stored = '0') then
                l_data_store(i2s_bit_width-1 downto 0) <= l_data_in;
                l_stored <= '1';
            end if;
        end if;
        if (l_sent = '1') then
            l_stored <= '0';
        end if;
    end process store_data_l;

    store_data_r : process(ws, clk, rst)
    begin
        if (rst = '0') then -- TODO reset on high or low?
            r_stored <= '0';
            r_data_store <= (others => '0');
        elsif rising_edge(ws) then
            if (r_stored = '0') then
                r_data_store(i2s_bit_width-1 downto 0) <= r_data_in;
                r_stored <= '1';
            end if;
        end if;
        if (r_sent = '1') then
            r_stored <= '0';
        end if;
    end process store_data_r;

    send_data : process(clk, rst)
    begin
        if (rst = '0') then
            l_valid <= '0';
            r_valid <= '0';
            l_data_out <= (others => '0');
            r_data_out <= (others => '0');
        elsif rising_edge(clk) then
            l_valid <= l_stored;
            r_valid <= r_stored;
            l_data_out <= l_data_store;
            r_data_out <= r_data_store;
        end if;
    end process send_data;

    update_send_status : process (clk, rst)
    begin
        if (rst = '0') then
            l_sent <= '0';
            r_sent <= '0';
        elsif falling_edge(clk) then
            if (l_stored = '1' and l_ready = '1' and l_sent = '0') then
                l_sent <= '1';
            elsif (l_stored = '0' and l_sent = '1') then
                l_sent <= '0';
            end if;
            if (r_stored = '1' and r_ready = '1' and r_sent = '0') then
                r_sent <= '1';
            elsif (r_stored = '0' and r_sent = '1') then
                r_sent <= '0';
            end if;
        end if;
    end process update_send_status;



end Behavioral;
