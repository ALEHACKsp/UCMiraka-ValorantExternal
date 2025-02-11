#include "imports.h"

auto cachethread() -> void
{
	auto guardedregion = driver.guarded_region();
	printf( "guardedregion: 0x%p\n", guardedregion );

	while (true)
	{
		auto uworld = utils::getuworld( guardedregion );
		printf( "uworld: 0x%p\n", uworld );

		auto ulevel = driver.read< uintptr_t >( uworld  + offsets::ulevel );
		if ( utils::isguarded ( ulevel ) )
		{
			auto _ulevel = driver.readguarded< uintptr_t >( guardedregion, uworld + offsets::ulevel );
			printf( "[readguarded result] ulevel: 0x%p\n", _ulevel );
		}
		else printf( "ulevel: 0x%p\n", ulevel );

		auto gamestate = driver.read< uintptr_t >( uworld + offsets::gamestate );
		if ( utils::isguarded( gamestate ) ) 
		{
			auto _gamestate = driver.readguarded< uintptr_t >( guardedregion, uworld + offsets::gamestate );
			printf( "[readguarded result] gamestate: 0x%p\n", _gamestate );
		}
		else printf( "gamestate: 0x%p\n", gamestate );

		Sleep( 2000 );
	}
}

auto main() -> const NTSTATUS
{
	auto process = utils::getprocessid( L"VALORANT-Win64-Shipping.exe" );

	printf( "processid: %i\n", process );

	if ( process != 0 )
	{
		driver.initdriver( process );
		std::thread(cachethread).detach();
	}

	getchar();
	return 0;
}
