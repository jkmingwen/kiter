library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity output_interface is
    generic (
        i2s_bit_width : natural := 24;
        fp_bit_width : natural := 34;
        bit_depth : natural := 2);
    port (
        clk : in std_logic;
        ws : in std_logic;
        rst : in std_logic;
        l_valid : in std_logic;
        r_valid : in std_logic;
        l_ready : out std_logic := '0';
        r_ready : out std_logic := '0';
        l_data_in : in std_logic_vector(i2s_bit_width - 1 downto 0);
        r_data_in : in std_logic_vector(i2s_bit_width - 1 downto 0);
        l_data_out : out std_logic_vector(i2s_bit_width - 1 downto 0) := (others => '0');
        r_data_out : out std_logic_vector(i2s_bit_width - 1 downto 0) := (others => '0') );
end output_interface;

architecture Behavioral of output_interface is

    signal l_data_store : std_logic_vector(i2s_bit_width - 1 downto 0) := (others => '0');
    signal r_data_store : std_logic_vector(i2s_bit_width - 1 downto 0) := (others => '0');

    signal l_stored : std_logic := '0';
    signal r_stored : std_logic := '0';

    signal l_sent : std_logic := '0';
    signal r_sent : std_logic := '0';

    signal l_sent_status : std_logic := '0';
    signal r_sent_status : std_logic := '0';

    signal l_is_ready : std_logic := '0';
    signal r_is_ready : std_logic := '0';

begin
    l_ready <= l_is_ready;
    r_ready <= r_is_ready;

    store_data : process(clk, rst)
    begin
        if (rst = '0') then
            l_is_ready <= '0';
            r_is_ready <= '0';
            l_data_store <= (others => '0');
            r_data_store <= (others => '0');
        elsif rising_edge(clk) then -- store new input data
            if (l_is_ready = '1' and l_valid = '1') then
                l_data_store <= l_data_in;
                l_stored <= '1';
                l_is_ready <= '0';
            end if;
            if (r_is_ready = '1' and r_valid = '1') then
                r_data_store <= r_data_in;
                r_stored <= '1';
                r_is_ready <= '0';
            end if;
        end if;

        if l_sent = '1' then
            l_stored <= '0';
        end if;
        if r_sent = '1' then
            r_stored <= '0';
        end if;

        if l_stored = '0' then
            l_is_ready <= '1';
        end if;
        if r_stored = '0' then
            r_is_ready <= '1';
        end if;
    end process store_data;


    update_send_status_l : process(ws, clk, rst)
    begin

       if r_stored = '0' then
            r_sent <= '0';
       end if;

        if rising_edge(ws) then -- at falling edge, data on l_data_out will be latched to sd_rx

            if r_stored = '1' then
                r_sent <= '1';
            end if;

            l_data_out <= l_data_store; -- for next falling edge
        end if;
    end process update_send_status_l;

    update_send_status_r : process(ws, clk, rst)
    begin

       if l_stored = '0' then
            l_sent <= '0';
       end if;


        if falling_edge(ws) then -- at rising edge, data on r_data_out will be latched to sd_rx

            if l_stored = '1' then
                l_sent <= '1';
            end if;

            r_data_out <= r_data_store; -- for next falling edge
        end if;
    end process update_send_status_r;

end Behavioral;
